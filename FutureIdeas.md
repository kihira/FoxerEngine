# Future Ideas
A bunch of ideas of how I could improve/tweak this in the future

### Physics
- Switch to a 3D physics library

### Scripting
- Preprocess lua files to convert Strings into SIDs for events etc
- Allow accessing event args by simply doing `event.argName`. Should be
possible via the use of meta_functions::index. See Entity

### Events
- Allow delaying execution of events

### Assets
- Create different loaders for each asset type
    - Along those lines, add stuff like getPhysicsComponent to entities from the respective subsystem
- Allow full overriding of components when loading for levels
- Only load data as defined in the level file
- Handle if asset is missing from database

### Rendering
- Make light a component so it's easier to sync and manipulate