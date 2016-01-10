BIN_DIR = node_modules/.bin/
JS_DIST_DIR = ./dist/

.PHONY: \
	all \
	dev \
	sed \
	install \
	clean \
	build \
	lint \
	lint-fix \
	server \
	nw \
	dev-no-debug \
	test \
	test-dev \
	deploy

all: dev

clean:
	@echo 'clean dist directory'
	@rm -rf dist;
	@echo "cleaned dist dir"

compile:
	@echo "start compilation process"
	@node -r dotenv/config --harmony bin/compile
	@echo "build completed in dist/"

build: closure-compile inline
	@echo "build finished"

install:
	@echo "start install using npm"
	@npm install
	@echo "install completed"

lint:
	@echo "start lint task"
	@${BIN_DIR}eslint . ./
	@echo "lint completed"

lint-fix:
	@echo "start lint and fix task"
	${BIN_DIR}eslint  --fix . ./
	@echo "lint-fix completed"

# run google closure library through npm
closure-compile: clean compile
	@echo "start closure compilation"
	@java \
		-jar node_modules/google-closure-compiler/compiler.jar \
		--language_in ECMASCRIPT5 \
		--compilation_level ADVANCED_OPTIMIZATIONS \
		--js ${JS_DIST_DIR}app.js ${JS_DIST_DIR}vendor.js \
		> ${JS_DIST_DIR}min.js
	@echo "closure compilation completed"

server:
	@echo "start node koa development server"
	node -r dotenv/config bin/server --hot

dev:
	@echo "spawn state debugger in a separate window"
	${BIN_DIR}nodemon -r dotenv/config bin/server --hot --nw

dev-no-nw:
	@echo "spawn state debugger inlined into page."
	${BIN_DIR}nodemon -r dotenv/config bin/server --hot

# start dev mode with production environment
dev-no-debug:
	@${BIN_DIR}nodemon -r dotenv/config bin/server --hot --no_debug

test:
	@echo 'start tests'
	@node -r dotenv/config ./node_modules/karma/bin/karma start bin/karma.js
	@echo 'tests finished'

test-dev:
	@node -r dotenv/config ./node_modules/karma/bin/karma start bin/karma.js --watch

inline:
	@node_modules/.bin/html-inline \
		-i ./dist/min.html \
		-o ./dist/bundle.html \
		-b ./dist/

deploy: test clean closure-compile inline

help:
	@echo " \n\
make [task] \n\
\n\
running make without task starts a dev env \n\
\n\
dev - start dev env \n\
install - install node dependencies \n\
clean - remove dist/ \n\
build - compile sources to dist dir \n\
lint - lint code using eslint \n\
lint-fix - lint and fix code \n\
server - run server only, no build chain \n\
nw - start browser history development tools in separate window \n\
dev-no-debug - start dev env in production mode \n\
test - run tests \n\
test-dev - run and watch tests \n\
deploy - test, clean then build \n\
"
