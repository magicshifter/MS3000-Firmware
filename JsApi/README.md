#MagicShifterApi

This library gives access to MagicShifter.js and MagicShifter.min.js,
a javascript class exposing the MagicShifter Api.

Installation:
```bash
  npm install magicshifter/MagicShifterApi
```

Usage:
es6/browserify/modules:
  ```js
    import MagicShifterApi from 'magicshifterapi';
  ```
es5:
  ```html
    <html>
      <head></head>
      <body>
        <script src="http://magicshifter.net/js/api/MagicShifter.js"></script>
        <script>
          var api = new window.MagicShifterApi();
          api.log('/leds');
        </script>
      </body>
    </html>
  ```
