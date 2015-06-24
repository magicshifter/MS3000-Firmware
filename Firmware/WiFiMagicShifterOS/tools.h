#ifndef __TOOLS_H
#define __TOOLS_H

// makes sure dest is \0 terminated
void safeStrncpy(char * dest, char *source, int n)
{
  strncpy(dest, source, n);
  dest[n-1] = '\0';
}

void loadString(char * str, int len)
{
  for (int i = 0; i < len; i++)
  {
    str[i] = EEPROM.read(i);
  }
  str[len-1] = '\0';
}

bool saveString(char * str, int len)
{
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(i, str[i]);
  }
  return EEPROM.commit();
}

#endif
