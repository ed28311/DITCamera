SET BASEDIR=%1
SET FILEPATH=%2
shift
shift
For %%A in ("%FILEPATH%") DO (
    SET FILEFOLDER=%%~dpA
    SET FILENAME=%%~nxA
)
start /D "%BASEDIR%" %FILEPATH% ^
-BL[Blemish] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\blemishNG.jpg ^
-BL[Blemish] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\blemishOK.jpg ^
-BL[Blemish] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\blemish.jpg ^
-FL[Flare] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\flareNG.jpg ^
-FL[Flare] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\flareOK.jpg ^
-FL[Flare] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\flare.jpg ^
-LS[Shading] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\shadingNG.jpg ^
-LS[Shading] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\shadingOK.jpg ^
-LS[Shading] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\shading.jpg