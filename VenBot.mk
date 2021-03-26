##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=VenBot
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=C:/Users/Venador/Documents/VenBot
ProjectPath            :=C:/Users/Venador/Documents/VenBot/VenBot
IntermediateDirectory  :=../build-$(ConfigurationName)/VenBot
OutDir                 :=../build-$(ConfigurationName)/VenBot
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Venador
Date                   :=25/03/2021
CodeLitePath           :=C:/Utilities/CodeLite
LinkerName             :=E:/Utilities/MinGW/bin/g++.exe
SharedObjectLinkerName :=E:/Utilities/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=..\build-$(ConfigurationName)\bin\$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=E:/Utilities/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := E:/Utilities/MinGW/bin/ar.exe rcu
CXX      := E:/Utilities/MinGW/bin/g++.exe
CC       := E:/Utilities/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := E:/Utilities/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Utilities\CodeLite
Objects0=../build-$(ConfigurationName)/VenBot/piece_functions.c$(ObjectSuffix) ../build-$(ConfigurationName)/VenBot/board_functions.c$(ObjectSuffix) ../build-$(ConfigurationName)/VenBot/xboard_interface.c$(ObjectSuffix) ../build-$(ConfigurationName)/VenBot/analysis.c$(ObjectSuffix) ../build-$(ConfigurationName)/VenBot/main.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/VenBot/.d $(Objects) 
	@if not exist "..\build-$(ConfigurationName)\VenBot" mkdir "..\build-$(ConfigurationName)\VenBot"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "..\build-$(ConfigurationName)\VenBot" mkdir "..\build-$(ConfigurationName)\VenBot"
	@if not exist ""..\build-$(ConfigurationName)\bin"" mkdir ""..\build-$(ConfigurationName)\bin""

../build-$(ConfigurationName)/VenBot/.d:
	@if not exist "..\build-$(ConfigurationName)\VenBot" mkdir "..\build-$(ConfigurationName)\VenBot"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/VenBot/piece_functions.c$(ObjectSuffix): piece_functions.c ../build-$(ConfigurationName)/VenBot/piece_functions.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Venador/Documents/VenBot/VenBot/piece_functions.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/piece_functions.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/VenBot/piece_functions.c$(DependSuffix): piece_functions.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/VenBot/piece_functions.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/VenBot/piece_functions.c$(DependSuffix) -MM piece_functions.c

../build-$(ConfigurationName)/VenBot/piece_functions.c$(PreprocessSuffix): piece_functions.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/VenBot/piece_functions.c$(PreprocessSuffix) piece_functions.c

../build-$(ConfigurationName)/VenBot/board_functions.c$(ObjectSuffix): board_functions.c ../build-$(ConfigurationName)/VenBot/board_functions.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Venador/Documents/VenBot/VenBot/board_functions.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/board_functions.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/VenBot/board_functions.c$(DependSuffix): board_functions.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/VenBot/board_functions.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/VenBot/board_functions.c$(DependSuffix) -MM board_functions.c

../build-$(ConfigurationName)/VenBot/board_functions.c$(PreprocessSuffix): board_functions.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/VenBot/board_functions.c$(PreprocessSuffix) board_functions.c

../build-$(ConfigurationName)/VenBot/xboard_interface.c$(ObjectSuffix): xboard_interface.c ../build-$(ConfigurationName)/VenBot/xboard_interface.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Venador/Documents/VenBot/VenBot/xboard_interface.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/xboard_interface.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/VenBot/xboard_interface.c$(DependSuffix): xboard_interface.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/VenBot/xboard_interface.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/VenBot/xboard_interface.c$(DependSuffix) -MM xboard_interface.c

../build-$(ConfigurationName)/VenBot/xboard_interface.c$(PreprocessSuffix): xboard_interface.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/VenBot/xboard_interface.c$(PreprocessSuffix) xboard_interface.c

../build-$(ConfigurationName)/VenBot/analysis.c$(ObjectSuffix): analysis.c ../build-$(ConfigurationName)/VenBot/analysis.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Venador/Documents/VenBot/VenBot/analysis.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/analysis.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/VenBot/analysis.c$(DependSuffix): analysis.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/VenBot/analysis.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/VenBot/analysis.c$(DependSuffix) -MM analysis.c

../build-$(ConfigurationName)/VenBot/analysis.c$(PreprocessSuffix): analysis.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/VenBot/analysis.c$(PreprocessSuffix) analysis.c

../build-$(ConfigurationName)/VenBot/main.c$(ObjectSuffix): main.c ../build-$(ConfigurationName)/VenBot/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Venador/Documents/VenBot/VenBot/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/VenBot/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/VenBot/main.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/VenBot/main.c$(DependSuffix) -MM main.c

../build-$(ConfigurationName)/VenBot/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/VenBot/main.c$(PreprocessSuffix) main.c


-include ../build-$(ConfigurationName)/VenBot//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


