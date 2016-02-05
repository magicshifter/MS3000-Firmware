from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

env.Replace(
#    UPLOAD_SPEED='921600'
#   UPLOAD_SPEED='460800'
#   UPLOAD_SPEED='230400'	
   UPLOAD_SPEED='115200'
)
