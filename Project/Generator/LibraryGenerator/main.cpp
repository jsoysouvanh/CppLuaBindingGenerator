#include <utility>	//std::forward
#include <vector>

#include <Kodgen/Misc/Filesystem.h>
#include <Kodgen/Misc/DefaultLogger.h>
#include <Kodgen/Parsing/FileParser.h>
#include <Kodgen/CodeGen/CodeGenManager.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnit.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h>

#include <RefurekuGenerator/CodeGen/ReflectionCodeGenModule.h>

fs::path getRootDirectory()
{
	fs::path path = fs::current_path();

	//Rewind until /CppLuaBindingGenerator directory
	while (path.has_stem() && path.stem() != "CppLuaBindingGenerator")
	{
		path = path.parent_path();
	}

	return path;
}

fs::path getLibraryDirectory()
{
	return getRootDirectory() / "Project" / "Library";
}

fs::path getGeneratorDirectory()
{
	return getRootDirectory() / "Project" / "Generator";
}

bool loadSettings(kodgen::CodeGenManagerSettings& codeGenMgrSettings, kodgen::ParsingSettings& parsingSettings,
				  kodgen::MacroCodeGenUnitSettings& codeGenUnitSettings, fs::path outputDirectory)
{
	bool result = true;

	fs::path libraryDirectoryPath = getLibraryDirectory();

	result &= codeGenMgrSettings.addSupportedFileExtension(".h");
	result &= codeGenMgrSettings.addSupportedFileExtension(".hpp");
	codeGenMgrSettings.addToProcessDirectory(libraryDirectoryPath / "Include" / "CppLuaBinding");
	codeGenMgrSettings.addIgnoredDirectory(outputDirectory);
	codeGenMgrSettings.addIgnoredFile(libraryDirectoryPath / "Include" / "CppLuaBinding" / "Config.h");

	result &= codeGenUnitSettings.setOutputDirectory(outputDirectory);
	codeGenUnitSettings.setGeneratedHeaderFileNamePattern("##FILENAME##.h.h");
	codeGenUnitSettings.setGeneratedSourceFileNamePattern("##FILENAME##.cpp.h");
	codeGenUnitSettings.setClassFooterMacroPattern("##CLASSFULLNAME##_GENERATED");
	codeGenUnitSettings.setHeaderFileFooterMacroPattern("File_##FILENAME##_GENERATED");

	parsingSettings.shouldAbortParsingOnFirstError = true;
	parsingSettings.shouldParseAllNamespaces = false;
	parsingSettings.shouldParseAllClasses = false;
	parsingSettings.shouldParseAllStructs = false;
	parsingSettings.shouldParseAllVariables = false;
	parsingSettings.shouldParseAllFields = false;
	parsingSettings.shouldParseAllFunctions = false;
	parsingSettings.shouldParseAllMethods = false;
	parsingSettings.shouldParseAllEnums = false;
	parsingSettings.shouldParseAllEnumValues = true;

	result &= parsingSettings.setCompilerExeName("clang++");

	parsingSettings.propertyParsingSettings.namespaceMacroName	= "NAMESPACE";
	parsingSettings.propertyParsingSettings.classMacroName		= "CLASS";
	parsingSettings.propertyParsingSettings.structMacroName		= "STRUCT";
	parsingSettings.propertyParsingSettings.variableMacroName	= "VARIABLE";
	parsingSettings.propertyParsingSettings.fieldMacroName		= "FIELD";
	parsingSettings.propertyParsingSettings.functionMacroName	= "FUNCTION";
	parsingSettings.propertyParsingSettings.methodMacroName		= "METHOD";
	parsingSettings.propertyParsingSettings.enumMacroName		= "ENUM";
	parsingSettings.propertyParsingSettings.enumValueMacroName	= "ENUMVALUE";

	return result;
}

void printGenerationResult(kodgen::ILogger& logger, kodgen::CodeGenResult const& genResult)
{
	if (genResult.completed)
	{
		logger.log("(Re)generated metadata for " + std::to_string(genResult.parsedFiles.size()) + " file(s) in " + std::to_string(genResult.duration) + " seconds.", kodgen::ILogger::ELogSeverity::Info);
		logger.log("Metadata of " + std::to_string(genResult.upToDateFiles.size()) + " file(s) up-to-date.", kodgen::ILogger::ELogSeverity::Info);
	}
	else
	{
		logger.log("Generation failed to complete successfully.", kodgen::ILogger::ELogSeverity::Error);
	}
}

void runWithRefurekuCompat(kodgen::ILogger& logger, kodgen::CodeGenManager& codeGenMgr, kodgen::FileParser& fileParser,
						   kodgen::MacroCodeGenUnit& codeGenUnit, kodgen::MacroCodeGenUnitSettings& codeGenUnitSettings)
{
	logger.log("Run with Refureku compatibility.");

	if (loadSettings(codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings, getLibraryDirectory() / "Include" / "CppLuaBinding" / "Generated"))
	{
		//Rfk specific setup
		codeGenUnitSettings.setGeneratedHeaderFileNamePattern("##FILENAME##_rfk_compat.h.h");
		codeGenUnitSettings.setGeneratedSourceFileNamePattern("##FILENAME##_rfk_compat.cpp.h");

		//Add Refureku library include
		fileParser.getSettings().addProjectIncludeDirectory(getLibraryDirectory() / "ThirdParty");

		rfk::ReflectionCodeGenModule reflectionCodeGenModule;
		codeGenUnit.addModule(reflectionCodeGenModule);

		//Parse
		kodgen::CodeGenResult genResult = codeGenMgr.run(fileParser, codeGenUnit, true);

		codeGenUnit.removeModule(reflectionCodeGenModule);

		//Result
		printGenerationResult(logger, genResult);
	}
	else
	{
		logger.log("Settings loading failed.", kodgen::ILogger::ELogSeverity::Error);
	}
}

void runWithoutRefurekuCompat(kodgen::ILogger& logger, kodgen::CodeGenManager& codeGenMgr, kodgen::FileParser& fileParser,
							  kodgen::MacroCodeGenUnit& codeGenUnit, kodgen::MacroCodeGenUnitSettings& codeGenUnitSettings)
{
	logger.log("Run without Refureku compatibility.");

	if (loadSettings(codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings, getLibraryDirectory() / "Include" / "CppLuaBinding" / "Generated"))
	{
		//Rfk specific setup
		codeGenUnitSettings.setGeneratedHeaderFileNamePattern("##FILENAME##_no_rfk_compat.h.h");
		codeGenUnitSettings.setGeneratedSourceFileNamePattern("##FILENAME##_no_rfk_compat.cpp.h");

		//Parse
		kodgen::CodeGenResult genResult = codeGenMgr.run(fileParser, codeGenUnit, true);

		//Result
		printGenerationResult(logger, genResult);
	}
	else
	{
		logger.log("Settings loading failed.", kodgen::ILogger::ELogSeverity::Error);
	}
}

int main()
{
	kodgen::DefaultLogger logger;

	kodgen::FileParser fileParser;
	fileParser.logger = &logger;

	kodgen::CodeGenManager codeGenMgr;
	codeGenMgr.logger = &logger;

	kodgen::MacroCodeGenUnit codeGenUnit;
	kodgen::MacroCodeGenUnitSettings codeGenUnitSettings;
	codeGenUnit.logger = &logger;
	codeGenUnit.setSettings(codeGenUnitSettings);

	//Load settings
	logger.log("Working Directory: " + fs::current_path().string(), kodgen::ILogger::ELogSeverity::Info);

	runWithRefurekuCompat(logger, codeGenMgr, fileParser, codeGenUnit, codeGenUnitSettings);
	runWithoutRefurekuCompat(logger, codeGenMgr, fileParser, codeGenUnit, codeGenUnitSettings);

	return EXIT_SUCCESS;
}