del /q /f ".\External\Include\Scripts\*.*"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Scripts\*.h" ".\External\Include\Scripts\"