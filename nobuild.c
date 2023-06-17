#define NOBUILD_IMPLEMENTATION
#include <nobuild.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define IMG_SIZE 512 * 1008

#define C_COMMON_FLAGS "-m32", "-Wall", "-Wextra", "-Iinclude", "-nostdlib", "-nostdinc", "-ffreestanding", "-fno-builtin", "-march=i386", "-no-pie", "-fno-pic", "--std=c11", "-Werror=implicit-function-declaration"
#define C_RELEASE_FLAGS "-O2", "-pedantic"
#define C_DEBUG_FLAGS "-ggdb"

#define ASM_COMMON_FLAGS "-march=i386", "--32"

#define LD_FLAGS "-nostdlib", "-no-pie", "-melf_i386", "--print-map" 

#define CC "i686-elf-gcc"
#define ASM "as"
#define LD "i686-elf-ld"

#define CSTR_ARRAY_APPEND(array, element) array = cstr_array_append(array, element);

typedef enum
{
    Release,
    Debug
} BuildType;

BuildType currentBuildType = Debug;
const char* buildFolder = "build/debug";
const char* kumirBuildFolder = "build/kumir";

const char* sourceFoldersToCompile[] = {
    "core",
    "kumlib"
};
const size_t sourceFoldersToCompileCount = (sizeof(sourceFoldersToCompile) / sizeof(sourceFoldersToCompile[0]));

void compileAsm(const char* filename, const char* outputFile)
{
    INFO("Compiling assembly file: %s -> %s", filename, outputFile);

    const char* outputPath = PATH(buildFolder, outputFile);

    if (currentBuildType == Release)
    {
        CMD(ASM, ASM_COMMON_FLAGS, "-c", filename, "-o", outputPath);
    }
    else if (currentBuildType == Debug)
    {
        CMD(ASM, ASM_COMMON_FLAGS, filename, "-o", outputPath);
    }
    else
    {
        PANIC("Unknown build type %d", currentBuildType);
    }

    free(outputPath);
}

void compileC(const char* filename, const char* outputFile)
{
    INFO("Compiling C file: %s -> %s", filename, outputFile);

    const char* outputPath = PATH(buildFolder, outputFile);

    if (currentBuildType == Release)
    {
        CMD(CC, C_COMMON_FLAGS, C_RELEASE_FLAGS, "-c", filename, "-o", outputPath);
    }
    else if (currentBuildType == Debug)
    {
        CMD(CC, C_COMMON_FLAGS, C_DEBUG_FLAGS, "-c", filename, "-o", outputPath);
    }
    else
    {
        PANIC("Unknown build type %d", currentBuildType);
    }

    free(outputPath);
}

void compileBootloader(void)
{
    compileAsm("src/boot/bootloader.s", "bootloader.s.o");
    compileAsm("src/boot/vbe.s", "vbe.s.o");
}

//size_t coreFilesCount = 2;
//char** coreFiles;

Cstr_Array coreFiles = { 0 };

void compileCore(void)
{
    //coreFiles = (char**)malloc(coreFilesCount * sizeof(char*));

    const char* test = sourceFoldersToCompile[0];

    for (size_t i = 0; i < sourceFoldersToCompileCount; i++)
    {
        const char* buildSubfolder = PATH(buildFolder, sourceFoldersToCompile[i]);
        if (!PATH_EXISTS(buildSubfolder))
        {
            MKDIRS(buildSubfolder);
        }
        free(buildSubfolder);

        const char* sourceFolder = PATH("src", sourceFoldersToCompile[i]);

        FOREACH_FILE_IN_DIR(file, sourceFolder, {
            if (ENDS_WITH(file, ".c"))
            {
                const char* relativeFilePath = PATH(sourceFoldersToCompile[i], file);
                const char* path = PATH("src", relativeFilePath);
                const char* outputFile = CONCAT(relativeFilePath, ".o");

                CSTR_ARRAY_APPEND(coreFiles, outputFile);
                //coreFiles[coreFilesCount - 2] = outputFile;
                //coreFilesCount++;
                //coreFiles = realloc(coreFiles, coreFilesCount * sizeof(char*));

                compileC(path, outputFile);

                free(path);
                free(relativeFilePath);
            }
            });
    }
}

Cstr_Array kumirFiles = { 0 };

void compileKumir(void)
{
    FOREACH_FILE_IN_DIR(file, "src", {
        if (ENDS_WITH(file, ".kum"))
        {
            const char* inputFile = PATH("src", file);
            const char* outputFile = CONCAT(file, ".o");
            const char* outputPath = PATH(kumirBuildFolder, outputFile);
            const char* outputFlag = CONCAT("-o=", outputPath);

            CMD("kumir2-llvmc", "-c", "-m", "-S", "-v", "-g=2", outputFlag, inputFile);

            free(inputFile);
            free(outputFlag);

            const char* outputLLFile = CONCAT(outputPath, ".ll");

            CMD("clang", C_COMMON_FLAGS, "-c", "-o", outputPath, outputLLFile);

            free(outputLLFile);

            CSTR_ARRAY_APPEND(kumirFiles, outputPath);
        }
        });
}

void linkCore(void)
{
    const char* outputFile = PATH(buildFolder, "kumiros.bin");

    Cstr_Array line = cstr_array_make(LD, LD_FLAGS, "-Tlinker.ld", NULL);

    const char* linkPath = getenv("LIBGCC_PATH");
    const char* linkParameter = CONCAT("-L", linkPath);


    CSTR_ARRAY_APPEND(line, PATH(buildFolder, "bootloader.s.o"));
    CSTR_ARRAY_APPEND(line, PATH(buildFolder, "vbe.s.o"));

    for (size_t i = 0; i < coreFiles.count; i++)
    {
        CSTR_ARRAY_APPEND(line, PATH(buildFolder, coreFiles.elems[i]));
    }

    for (size_t i = 0; i < kumirFiles.count; i++)
    {
        CSTR_ARRAY_APPEND(line, kumirFiles.elems[i]);
    }

    CSTR_ARRAY_APPEND(line, "-o");
    CSTR_ARRAY_APPEND(line, outputFile);
    CSTR_ARRAY_APPEND(line, linkParameter);
    CSTR_ARRAY_APPEND(line, "-lgcc");

    Cmd cmd = {
        .line = line
    };

    INFO("Linking core");
    INFO("Run: %s", cmd_show(cmd));

    cmd_run_sync(cmd);

    free(outputFile);
    free(linkParameter);
}

void makeImg(void)
{
    INFO("Copying linked core to image");

    const char* buildTypeString = "debug";
    if (currentBuildType == Release) buildTypeString = "release";

    const char* outputFile = CONCAT(buildTypeString, ".img");

    FILE* image = fopen(outputFile, "wb");
    if (!image)
    {
        PANIC("Can't create image file");
    }

    {
        const char* corePath = PATH(buildFolder, "kumiros.bin");
        FILE* coreFile = fopen(corePath, "rb");

        struct stat st;
        fstat(fileno(coreFile), &st);

        uint8_t* buffer = malloc(IMG_SIZE);
        fread(buffer, sizeof(uint8_t), st.st_size, coreFile);
        fwrite(buffer, sizeof(uint8_t), IMG_SIZE, image);

        fclose(coreFile);
        free(corePath);
    }

    INFO("Successfuly written image to %s", outputFile);

    fclose(image);
    free(outputFile);
}

int main(int argc, char** argv)
{
    if (!getenv("LIBGCC_PATH"))
    {
        PANIC("LIBGCC_PATH not found!");
    }

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "debug"))
        {
            currentBuildType = Debug;
        }
        else if (!strcmp(argv[i], "release"))
        {
            currentBuildType = Release;
        }
    }

    if (currentBuildType == Release) buildFolder = "build/release";
    if (currentBuildType == Debug) buildFolder = "build/debug";

    if (!PATH_EXISTS(buildFolder))
    {
        MKDIRS(buildFolder);
    }

    if (!PATH_EXISTS(kumirBuildFolder))
    {
        MKDIRS(kumirBuildFolder);
    }

    INFO("Build folder: %s", buildFolder);

    compileBootloader();
    compileCore();
    compileKumir();

    linkCore();

    makeImg();
}