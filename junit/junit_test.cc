/*
    Copy this code in to your application, replacing the test cases in the runTest function.
    Then call runTest() from somewhere in your application.
*/
#include <tracktion_engine/tracktion_engine.h>
#include <filesystem>
namespace te = tracktion;

class TestClassName  : public juce::UnitTest
{
public:
    TestClassName()
        : juce::UnitTest ("TestClass", "tracktion_engine")
    {
    }

    void runTest() override
    {
        // Group tests in to sections with beginTest
        beginTest ("Test section");
        {
            const juce::ScopedJuceInitialiser_GUI initialiser; // need this

            te::Engine engine{"Tracktion Hello World"};
            std::filesystem::path curr_path = std::filesystem::current_path();
            juce::File my_file{juce::String{curr_path.string() + "/tmp.box"}};

            std::unique_ptr<te::Edit> edit = createEmptyEdit(engine, my_file);
            edit->ensureNumberOfAudioTracks(8);
            edit->getTransport().ensureContextAllocated();

            {
                auto &dm = engine.getDeviceManager();

                for (int i = 0; i < dm.getNumWaveInDevices(); i++)
                {
                    if (auto wip = dm.getWaveInDevice(i))
                    {
                        wip->setStereoPair(false);
                        // wip->setEndToEnd(true); // TODO update ?
                        wip->setEnabled(true);
                    }
                    else if (auto mip = dm.getMidiInDevice(i))
                    {
                        // mip->setEndToEndEnabled(true); // TODO update ?
                        mip->setEnabled(true);
                    }
                }
            }


            engine.getDeviceManager().createVirtualMidiDevice("box_midi");
            engine.getDeviceManager().setDefaultMidiInDevice("box_midi");
            for (int i = 0; i < 10000; i++)
            {
                auto ptr = engine.getDeviceManager().getDefaultMidiInDevice();
                expect(ptr != nullptr);
            }

        }

        beginTest("print midi");
        {
            const juce::ScopedJuceInitialiser_GUI initialiser; // need this
            te::Engine engine{"Tracktion Hello World"};
            std::filesystem::path curr_path = std::filesystem::current_path();
            juce::File my_file{juce::String{curr_path.string() + "PrintMidiIn.box"}};
            std::unique_ptr<te::Edit> edit = createEmptyEdit(engine, my_file);
            edit->ensureNumberOfAudioTracks(8);
            edit->getTransport().ensureContextAllocated();


            auto vec = engine.getDeviceManager().getMidiInDevices();
            std::cout << "midiInputs.size(): " << vec.size() << std::endl;
            for (auto &x : vec)
            {
                std::cout << x->getName() << std::endl;
            }
        }
    }
};

// Creates an instance of the test so you can run it
static TestClassName testClassName;

// Call this from your applications
inline void runTest()
{
    juce::UnitTestRunner runner;
    runner.runTests ({ &testClassName });
}

int main()
{
    const juce::ScopedJuceInitialiser_GUI initialiser; // need this

    te::Engine engine{"Tracktion Hello World"};
    std::filesystem::path curr_path = std::filesystem::current_path();
    juce::File my_file{juce::String{curr_path.string() + "/tmp.box"}};

    std::unique_ptr<te::Edit> my_edit = createEmptyEdit(engine, my_file);
    te::Edit &edit = *my_edit;
    edit.ensureNumberOfAudioTracks(8);
    edit.getTransport().ensureContextAllocated();

    // arm midi
    {
        for (auto instance : edit.getAllInputDevices())
        {
            auto device_type = instance->getInputDevice().getDeviceType();
            if (device_type == te::InputDevice::physicalMidiDevice || device_type == te::InputDevice::virtualMidiDevice)
            {
                auto t = te::getAudioTracks(edit)[0];
                if (t != nullptr)
                {
                    [[maybe_unused]] auto res = instance->setTarget(t->itemID, true, &edit.getUndoManager(), 0);
                    instance->setRecordingEnabled(t->itemID, true);
                }
            }
        }
    }


    try
    {
        // arm
        {
            auto &dm = engine.getDeviceManager();

            for (int i = 0; i < dm.getNumWaveInDevices(); i++)
            {
                if (auto wip = dm.getWaveInDevice(i))
                {
                    wip->setStereoPair(false);
                    // wip->setEndToEnd(true); // TODO update ?
                    wip->setEnabled(true);
                }
                else if (auto mip = dm.getMidiInDevice(i))
                {
                    // mip->setEndToEndEnabled(true); // TODO update ?
                    mip->setEnabled(true);
                }
            }
        }


                // engine.getDeviceManager().createVirtualMidiDevice("box_midi");
        // engine.getDeviceManager().setDefaultMidiInDevice("box_midi");
        // while (true) {
            auto vec = engine.getDeviceManager().getMidiInDevices();
            std::cout << "midiInputs.size(): " << vec.size() << std::endl;
            for (auto &x : vec)
            {
                std::cout << x->getName() << std::endl;
            }
        // }
        
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    // runTest();
    return 0;
}
