#ifndef _EEPROM_STRING_URL_H
#define _EEPROM_STRING_URL_H

#define BASE64_ENC_LEN(n) (((n) + 2 - (((n) + 2) % 3)) / 3 * 4)


class MagicShifterEEPROMString {

  public:
	// makes sure dest is \0 terminated
	void safeStrncpy(char *dest, const char *source, int n) {
		strncpy(dest, source, n);
		dest[n - 1] = '\0';
	} void loadString(char *str, int len) {
		for (int i = 0; i < len; i++) {
			str[i] = EEPROM.read(i);
		}
		str[len - 1] = '\0';
	}

	bool saveString(char *str, int len) {
		for (int i = 0; i < len; i++) {
			EEPROM.write(i, str[i]);
		}
		return EEPROM.commit();
	}



	bool memcmpByte(const byte * data, byte referenceValue, int len) {
		for (int i = 0; i < len; i++) {
			if (data[i] != referenceValue)
				return false;
		}
		return true;
	}
  private:

	int hex_decode(char hex) {
		if (hex >= '0' && hex <= '9')
			return hex - '0';
		if (hex >= 'a' && hex <= 'z')
			return 10 + hex - 'a';
		if (hex >= 'A' && hex <= 'Z')
			return 10 + hex - 'A';

		return 0;
	}

	int url_decode(char *output, const char *input, int maxLen) {
		int l = 0;
		char c;

		while (*input != 0 && l < maxLen) {
			if (*input == '+') {
				c = ' ';
				input++;
			} else if (*input == '%') {
				input++;
				c = 16 * hex_decode(*input);
				input++;
				c += hex_decode(*input);
				input++;
			} else {
				c = *input;
				input++;
			}
			*output = c;

			l++;
			output++;
		}
		*output = '\0';
		return l;
	}

};

#endif
