BIN_DIR = node_modules/.bin/

.PHONY: \
	all \
	dev \
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
	rm -rf dist;

build:
	node -r dotenv/config --harmony bin/compile

install:
	npm install

lint:
	${BIN_DIR}eslint . ./

lint-fix:
	${BIN_DIR}eslint  --fix . ./

server:
	node -r dotenv/config bin/server --hot

dev:
	${BIN_DIR}nodemon -r dotenv/config bin/server --hot

nw:
	${BIN_DIR}nodemon -r dotenv/config bin/server --hot --nw

dev-no-debug:
	@${BIN_DIR}nodemon -r dotenv/config bin/server --hot --no_debug

test:
	@node -r dotenv/config ./node_modules/karma/bin/karma start bin/karma.js

test-dev:
	@node -r dotenv/config ./node_modules/karma/bin/karma start bin/karma.js --watch

deploy: test clean build

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
