# [Computational Depth of Field - Jiacong Li](https://gitlab.ewi.tudelft.nl/cgv/cs4365/student-repositories/2023-2024/cs436523jiacongli/-/tree/main)
## 1.File structure


| name | type | description |
| -------- | -------- | -------- |
| input     | folder     | input images     |
| output     | folder     | intermediate and final output images     |
| main     | cpp     |main code running UI and all functions |
| Diffusion     | h     | anisotropic diffusion function codes     |
| Cross_Bilateral_Filter    | h     | cross bilateral filter function codes     |
| UI     | h     | UI codes     |

For more detailed info of codes, please check section 3.

## 2.How to run the code
All codes were worte and run on **Windows 10**, with **visual studio code**, using **OpenCV** library. Becasue I am not sure if it can run on other platform, I will write as detailed as possible about how I set up the environment to make sure it can run.
### 2.1 Set up
#### 1) Download OpenCV
Download OpenCV library from the following link: https://opencv.org/releases/. Version 4.8.0 for Windows is used in my case.

![image.png](https://hackmd.io/_uploads/HJZ5BJB7T.png)

After downloading, you will get a executable file. Open it and choose a folder for extacting. After extracting you will have a folder called 'opencv'.

#### 2) Setup system path for OpenCV
Search "Edit the system environment variables" in Windows.

![image.png](https://hackmd.io/_uploads/S1dyw1H7T.png)

When you click it you will get an interface like this:

![image.png](https://hackmd.io/_uploads/SkdDDJrmT.png)

Click "Environment Variables", you will get an interface like this:

![image.png](https://hackmd.io/_uploads/r1CruySQa.png)

Double click 'Path' in "System variables":

![image.png](https://hackmd.io/_uploads/S1DMOJBmT.png)

Click 'New' and then click 'Browse', and choose "YOURPATH/opencv/build/x64/vc16/bin". Click 'OK'. Then the path is added. You need to do the same thing for second times to add "YOURPATH/opencv/build/x64/vc16/lib". The following figure shows which two folders you should add.

![image.png](https://hackmd.io/_uploads/SyQvYyH76.png)

It's good to note that, by default, you have installed Cmake and added "Cmake/bin" folder to the environment variables. If not, please download it from the following URL https://cmake.org/download/ and select "Add CMake to the system PATH for all users" during installation:

![image.png](https://hackmd.io/_uploads/r1Ia5kBm6.png)

![image.png](https://hackmd.io/_uploads/rk2ljkr7T.png)

Finally, you will have a path list like following:

![image.png](https://hackmd.io/_uploads/BkYrj1rmp.png)

Press 'OK's until every above mentioned tables is closed.

#### 3) Set up project in Visual Studio Code
Please install Visual Studio Code first. If you don't have it. you can download from this URL: https://code.visualstudio.com/Download.

For VS Code extensions, you need to install C/C++ and Cmake Tools extensions:

![image.png](https://hackmd.io/_uploads/rJDBAkrQp.png)

![image.png](https://hackmd.io/_uploads/rkBDRkrXT.png)

After installing CMake Tools. Press 'Shift' + 'Ctrl' + 'p', you will be guided to a search interface. Type "Select a Kit" and press 'Enter'

![image.png](https://hackmd.io/_uploads/Bkv0CkB76.png)

![image.png](https://hackmd.io/_uploads/Skm4JgHm6.png)

And then choose a kit for your system. In my case I chose the last on for x86_amd64.

After that, open the code folder in VS code, you should have following files and folders as describe in the first section

![image.png](https://hackmd.io/_uploads/Bk6BxlSmp.png)

Open the 'CmakeLists.txt' file and press 'Ctrl' + 's'. The VS code will make the project automatically. After that you should get something like this in the terminal, with your own file path:

![image.png](https://hackmd.io/_uploads/SJcTegSmp.png)

### 2.2 Run the code
It is simple to run the code after you setting it up. After you install CMake Tools in VS code. There should be a start button (triangle) at the bottom of your VS code GUI.

![image.png](https://hackmd.io/_uploads/H1XzzgH7T.png)


Press it, after a while, you will be asked to input an input image path. For example you can use the input given in the input folder:
"../../input/church.jpg"

After input the path, you will can edit the image.
![image.png](https://hackmd.io/_uploads/HyXyQeSQp.png)


## 3.Code structure
### 3.1 main.cpp
There are two functions in main.cpp


| function | location | description |
| -------- | -------- | -------- |
| onMouse     | line 33     | The function used for mouse movement capture  |
| main     | line 65     | Main function for UI, user input capture , scribble diffusion and imgae blurring  |

### 3.2 Diffusion.h (algorithmic steps)

| function | location | description |
| -------- | -------- | -------- |
| anisotropic     | line 18     | Scribble diffusion function using [anisotropic diffusion](https://graphics.tudelft.nl/Publications-new/2017/LSE17a/depthannotations-authorsversion.pdf)   |

### 3.3 Cross_Bilateral_Filter.h (algorithmic steps)
| function | location | description |
| -------- | -------- | -------- |
| gauss     | line 4     | PDF calculation of a normal distribution  |
| cross_bilateral_filter     | line 24     | Blurring image with [cross bilateral filter](https://graphics.tudelft.nl/Publications-new/2004/ED04a/flash.pdf) according to user-set focus point, aperture and depth map  |

### 3.4 UI.h
| function | location | description |
| -------- | -------- | -------- |
| default_txt     | line 9     | Fixed shown text in UI  |
| run_UI     | line 71     | Run UI and update UI text  |
| start_ui     | line 106   | Start state of UI  |







[](url)
