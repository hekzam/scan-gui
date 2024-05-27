# Hekzam GUI v0.1.0

## Running the program with the provided examples
Either through Qt Creator after importing the project (easy) 
or running ./scan-gui after building.
- **If you haven't ran the program before**:
    - Click `Create a New Project`
    - In Project Repository : Select a directory to store the recovery data (useful for testing 
as you'll only need to pass this JSON file to open all previous files)
    - In Exam Data : select all or some of the files from `resources/test_case/Json/`
    - In Scan Files : select all or some of the files from `resources/test_case/Fichiers`
    - last field is left empty for now
- **If you did run the program before**
    - Click `Open an Existing Project`
    - Open the `data.json` file from the directory you specified in Project Repository.

Then you can :
 - Click on an entry in the table
 - Clicking on different columns yield different results
 - Look at the Application output for hints on what still needs to be done.
 - read the summaries of the modules located in the .h files (mainly `preview`, 
 `sortTable`, `TableBox` and `mainwindow`)
 - Hovering the sliders below the preview opens up a tooltip that tells you what they do.
 - some buttons do not work : Delete, Assing and Mark as Validated, 
 feel free to remove, rework them if needed.
## What can be Done
- There are things still not implemented due to the lack of backend at the time
this project was conducted. Some code is therefore going to be removed.
- Items displayed in pages aren't positioned where they seem, this could be 
improved/changed (see `markeritem.cpp`). The fieldItems are initialized in 
 `setFieldItemAttributes` in `singlepage.cpp`
- The mask applied on the preview (see `pagemask.cpp`, `addFieldToHighlight` 
in `pagemask.cpp`, `FieldItems` and `singlepage.cpp`)
depends on the `m_rect` values of the fieldItems but changing 
around values in the coordinate system might break things on that end too.
