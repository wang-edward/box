# doing?
[*] Selector should be accessed through Track, not through Manager
[ ] Use PluginConfig instead of Plugin
    - this holds data like the icon and the name and stuff
    - and then we can only have 

# formatting stuff to keep in mind (virus type spread)
- make class into struct 
- put { on nextline of func def
- same thing for if? cuz when u have a long if statement its not obvious when the conditional ends and the code begins
- dont indent after switch

# SHA
- Rewrite Parameter to be better
- has getNorm for CV

# todo
## easy
- make Plugin a struct and GetPlugin to plugin->base;
- make const vectors arrays
- figure out some way to better assert_tracks or whatever
    - like how do i handle when an array of things has len 0
- handle tracks and base_tracks better
    - should be allocated at the start?
- DrawCircleSector() for Knob


## long term
- patchbay?
- distinction between instrument and effect? like having seperate slots for types of plugins
- better way to manage colorschemes - MAINCOLOR, ALT1, ALT2, etc.
- better way to manage plugins - define stuff all in one place
- timeline refactor - should Timeline == Manager?
- store icon in static plugin, uses way too much memory for no reason

# timeline
- timer turns red when recording
- timer turns grey when all tracks are muted
- timer turns yellow when a track is solo'ed
