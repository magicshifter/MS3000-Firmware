from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

LOCAL_UPLOADER="~/Documents/esptool/esptool.py"

env.Replace(
    LOCAL_UPLOADERFLAGS=[
        "-p", "$UPLOAD_PORT",
        "-b", "921600",
        "-ff", "40m", 
        "-fm", "dio",
        "write_flash", "0x100000",
    ],
    UPLOADER=LOCAL_UPLOADER,
    UPLOADCMD='$UPLOADER $LOCAL_UPLOADERFLAGS $SOURCE',
)
