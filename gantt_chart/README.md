# gantt_chart

[Link to Design Document](https://docs.google.com/document/d/1ty2MV69mA_ILQX2LADg-Ld0-z0Imx26X0hiAM7Uw8Xw/edit?usp=sharing)

## Design Requirements
- [x] Gantt displayed correctly and has a sample task list to start with (15 points)

- [x] Git Repo is listed within the text file (5 points)
- [ ] The code is well commented and appropriately divided into functions (10 points)
- [x] You have created an interface to interact with the Gantt chart. (15 points)
- [ ] The submitted text file describes your design choices appropriately (10 points)
- [x] You can input a new task list for the Gantt to show (15 points)

- [x] You can edit a task using its name on the Gantt Chart and show the resulting change. (10 points)
- [x] You must write a way to test the Gantt diagrams for dependencies to make sure that no task is impossible. This will be based off its own dependencies, you do not need to take into account the timelines of the project in terms of points (15 points)
- [x] Adding a personalised piece of ascii art to the main c file (5 points)

## Run Requirements
* cmake
* Ninja

### Initialize
> `cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=clang`

### Build & Run
> `cmake --build build & .\build\gantt_chart.exe`
