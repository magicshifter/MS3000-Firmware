from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

env.Replace(
   UPLOAD_SPEED='921600'
)
