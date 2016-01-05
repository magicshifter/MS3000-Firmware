# EvoCell Environment
DIST_DIR=./dist/
JS_DIST_DIR=${DIST_DIR}js/
CSS_DIST_DIR=${DIST_DIR}css/

.PHONY: \
	server \
	browserify \
	browserify-sourcemap \
	watch \
	watch-sourcemap \
	clean \
	stylus \
	help \
	;

# a simple make starts the watch task
all: watch

build: ; ${MAKE} -j3 browserify build-assets stylus

# copies ./src/assets, git clones and then starts server
server-static:
	cd ${DIST_DIR} && \
	python -m SimpleHTTPServer
	;

server-port80:
	cd ${DIST_DIR} && \
	sudo python -m SimpleHTTPServer 80
	;

server-lr:
	node_modules/.bin/livereload ${DIST_DIR};

server: \
	; @${MAKE} \
		-j 2 \
		server-static #server-lr

# installs node modules 
install:
	npm install;

# installs inotify on linux
install-inotify:
	sudo apt-get install inotify-tools;

# builds js sources into dist/js/build.js
browserify:
	mkdir -p ${JS_DIST_DIR};
	node_modules/.bin/browserify \
	-e src/index.js \
	-t [ babelify --presets [es2015 react] --plugins [transform-class-properties] ] \
	-o ${JS_DIST_DIR}build.js \
	;
	@echo "javascript built";

# build js sources and include sourcemaps
browserify-sourcemap:
	@mkdir -p ${JS_DIST_DIR};
	@node_modules/.bin/browserify \
	-e src/index.js \
	-t [ babelify --presets [es2015 react] ] \
	-p [ babel-plugin-transform-class-properties ] \
	--debug \
	| node_modules/.bin/exorcist ${JS_DIST_DIR}bundle.js.map > ${JS_DIST_DIR}build.js \
	;
	@echo "javascript built";

# watch javascript and rebuild on change
watch-javascript:
	@mkdir -p ${JS_DIST_DIR};
	@echo "start watching javascript";
	@node_modules/.bin/watchify \
		-e src/index.js \
		-t [ babelify --presets [es2015 react] --plugins [transform-class-properties] ] \
		-o ${JS_DIST_DIR}build.js \
		-v \
	;

# watch javascript and rebuild on change, include sourcemaps
watch-sourcemap:
	mkdir -p ${JS_DIST_DIR};
	node_modules/.bin/watchify \
	-e src/index.js \
	-o ${JS_DIST_DIR}build.js \
	-t [ babelify --presets [es2015 react] ] \
	--debug \
	-v \
	| node_modules/.bin/exorcist ${JS_DIST_DIR}bundle.js.map > ${JS_DIST_DIR}build.js \
	;

# watch static files and recopy/rebuild on change

#	does not run on mac os x, no inotifywait there
watch-assets:
	@echo "watch static file assets and css";
	@while inotifywait -r \
		-e close_write \
			./src/assets/* ./src/css/*; do make build-static; \
	done;

watch: ; @${MAKE} -j2 watch-javascript watch-assets;

watch-assets-osx: build-static
	@echo "watch static file assets and css"
	@node watch.js

watch-osx: ; @${MAKE} -j2 watch-javascript watch-assets-osx;

# run uglifyjs task
uglify:
	@node_modules/.bin/uglifyjs \
		${JS_DIST_DIR}build.js \
		--mangle -c -o \
		${JS_DIST_DIR}build.min.js;
	@echo "uglify done";

# run google closure library through npm
closure-compile:
	java \
		-jar node_modules/google-closure-compiler/compiler.jar \
		--language_in ECMASCRIPT5 \
		--compilation_level ADVANCED_OPTIMIZATIONS \
		--js ${JS_DIST_DIR}build.js \
		> ${JS_DIST_DIR}build.closure.js \
	;


# copy static files to ./dist
build-assets:
	mkdir -p ${DIST_DIR}
	cp -rf ./src/assets/* ${DIST_DIR};

# build stylus to dist/style.css
stylus:
	@mkdir -p ${CSS_DIST_DIR};
	@node_modules/.bin/stylus \
	< src/css/style.styl \
	> ${CSS_DIST_DIR}style.css \
	;
	@echo "stylus build finished"

# remove dist directory
clean:
	rm -rf ${DIST_DIR};

lint:
	@node_modules/.bin/jscs \
		--reporter=console \
		src;

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
";


