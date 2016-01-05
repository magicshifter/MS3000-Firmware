# EvoCell Environment
DIST_DIR=./dist/
JS_DIST_DIR=${DIST_DIR}js/
CSS_DIST_DIR=${DIST_DIR}css/

.PHONY: \
	dev \
	all \
	install \
	install-inotify \
	build \
	clean \
	help

# a simple make starts the watch task
all: dev

dev:
	@node server.js

# installs node modules 
install:
	@npm install

# installs inotify on linux
install-inotify:
	@sudo apt-get install inotify-tools

jade:
	@node_modules/.bin/jade \
		-o dist/ \
		src/html/ 

build: clean jade
	@node_modules/.bin/cross-env NODE_ENV=production \
		node_modules/.bin/webpack --config webpack.config.prod.js;
	@echo "javascript built";

# run google closure library through npm
closure-compile:
	@java \
		-jar node_modules/google-closure-compiler/compiler.jar \
		--language_in ECMASCRIPT5 \
		--compilation_level ADVANCED_OPTIMIZATIONS \
		--js ${JS_DIST_DIR}bundle.js \
		> ${JS_DIST_DIR}bundle.closure.js

# remove dist directory
clean:
	@rm -rf ${DIST_DIR};
	@echo 'dist directory deleted'

lint:
	@node_modules/.bin/jscs \
		--reporter=console \
		src

#output some information
help:
	@echo "\
to get a development environment you can \n\
make watch \n\
\n\
server - python SimpleHTTPServer \n\
build-static - copies static-files and builds stylus \n\
build-assets - copy ./src/assets/* to ./dist/ \n\
stylus - build css from ./src/css/style.styl to ./dist/css/style.css \n\
install - install javascript dependencies \n\
browserify - builds javascript sources \n\
browserify-sourcemap - builds javascript sources and sourcemaps \n\
watch - continuously rebuilds sources \n\
watch-javascript - continuously rebuilds js \n\
watch-sourcemap - continuously rebuilds javascript inc sourcemaps \n\
watch-assets - builds css and copies static files using inotifywait and build-static \n\
watch-assets-osx - builds css and copies static files using javascript on mac os x \n\
uglify - uglify javascript - closure-compiler is better \n\
closure-compile - minify and dead code eliminate javascript \n\
help - this help output \n\
"


