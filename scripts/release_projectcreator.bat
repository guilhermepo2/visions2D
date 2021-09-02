SET project_creator_src=..\bin\Debug-windows.x86_64\project-creator\project-creator.exe
SET project_creator_dst=..\tools\project-creator\bin\project-creator.exe

xcopy /s %project_creator_src% %project_creator_dst%
PAUSE
