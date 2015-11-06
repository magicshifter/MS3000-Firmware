# ms3000

MagicShifter 3000 clojurescript ui

## Setup
```bash
   #get this repository
   cd clojure
   lein hiccup-watch
   #open a second terminal while hiccup-watch is running
   touch resources/hiccup/index.clj
   #close that terminal if hiccup-watch tells you it just rebuilt your html
   ./dev
   #now the figwheel dev environment described below will start.
   #have a look at the HTML and CSS sections below if you want to work on html or css.
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

#### HTML using hiccup
To start building the html open an additional terminal:
```bash
lein hiccup-watch
```
starts a watch task that watches for changes to the resources/hiccup clj files.
so, to get the index.html rendered for the first time:
```bash
touch resources/hiccup/index.clj
```

#### CSS using garden
To start building the css with garden:
```bash
lein garden auto dev
```