# doing?
- #11 print timeline view

# Track::Plugin view
- deleting plugins causes it to shrink down
- you can only put active_plugin_ on an index that exists

# formatting stuff to keep in mind (virus type spread)
- make class into struct 
- put { on nextline of func def
- same thing for if? cuz when u have a long if statement its not obvious when the conditional ends and the code begins
- dont indent after switch

# future stuff to keep in mind
- switch state machine doesn't work great when there are 2 paths to a screen
    - which one do you go back to?
- What is the state machine hierarchy?
    - right now it goes ScreenState -> Mode -> EventType -> event.value
    - but maybe it should be Mode first?
- find a linter that catches switch fallthrough
    - this is gonna be a huge issue when the paths get big
- Pots should be excluded from modal stuff. That way you can actually adjust the params while you are playing notes
- some log scale for Norm

# existential issues
- does a modal daw even make sense :joy: :joy: :joy: you can't navigate menus while playing notes. but is that even the optimal workflow???

# stuff im NOT SURE if i should do
[ ] Use PluginConfig instead of Plugin
    - this holds data like the icon and the name and stuff
    - and then we can only have 

# todo
## easy
- make const vectors arrays
- figure out some way to better assert_tracks or whatever
    - like how do i handle when an array of things has len 0
- handle tracks and base_tracks better
    - should be allocated at the start?

## long term
- get rid of Interface, it doesn't really do anything
- patchbay?
- distinction between instrument and effect? like having seperate slots for types of plugins
- better way to manage colorschemes - MAINCOLOR, ALT1, ALT2, etc.
- better way to manage plugins - define stuff all in one place
- store icon in static plugin, uses way too much memory for no reason

# timeline
- timer turns red when recording
- timer turns grey when all tracks are muted
- timer turns yellow when a track is solo'ed
