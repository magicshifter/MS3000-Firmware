# Magic Client

this application connects to all your magic devices.

## Setup
```bash
   #get this repository
   cd clojure/MagicClient
   
   #weird hiccup-watch dance to create initial html:
   lein hiccup-watch
   #open a second terminal while hiccup-watch is running
   touch resources/hiccup/index.clj
   
   #close that terminal if hiccup-watch tells you that it just rebuilt your html
   ./dev
   #now the figwheel dev environment described below will start.
   #have a look at the HTML and CSS sections below if you want to work on html or css as well.
   
   # if you want to preview your changes in nodewebkit instead of your browser,
   # open another terminal and, whilst in this directory, type:
   npm install
   #after installing, start nodewebkit using:
   npm start
```


#### Clojurescript Development

To get an interactive development environment run:
```bash
    ./dev
```
and open your browser at [localhost:3449](http://localhost:3449/).
This will auto compile all *javascript* and send all changes to the browser without the
need to reload. After the compilation process is complete, you will
get a Browser Connected REPL. An easy way to try it is:

    (js/alert "Am I connected?")

and you should see an alert in the browser window.

To clean all compiled files:
```bash
lein clean
```

#### HTML development using hiccup
To start building the html open an additional terminal:
```bash
lein hiccup-watch
```
starts a watch task that watches for changes to the resources/hiccup clj files.
so, to get the index.html rendered for the first time:
```bash
touch resources/hiccup/index.clj
```

#### CSS development using garden
To start building the css with garden:
```bash
lein garden auto dev
```