#ifndef _MSAPINFO_H
#define  _MSAPINFO_H

#define MAX_AP_LEN 48
struct APInfo
{
    char ssid[MAX_AP_LEN];
    char password[MAX_AP_LEN];

  public:
    APInfo()
    {
      clear();
    }

    void clear(void)
    {
      memset(ssid, 0, sizeof(ssid));
      memset(password, 0, sizeof(password));
    }
};


struct ServerConfig
{
  char hostname[48];
  int port;

public:
  ServerConfig()
  {
    clear();
  }

  void clear(void)
  {
    memset(hostname, 0, sizeof(hostname));
    port = 80;
  }
};

struct APConfig
{
  struct APInfo  apInfo;
  bool forceAPMode;
};

#endif
