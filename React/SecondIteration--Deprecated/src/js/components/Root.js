import React, {Component, PropTypes} from 'react';
import { applyMiddleware, compose, createStore, combineReducers } from 'redux';
import { Provider } from 'react-redux';
import { Router, Route, IndexRoute } from 'react-router';
import createHistory from 'history/lib/createHashHistory';
import { syncHistory, routeReducer as routing} from 'redux-simple-router';

import Radium from 'radium';

import App from '../containers/App';

import Header from './Header';
import About from './About/About';
import PixelEditor from './PixelEditor/PixelEditor';
import Settings from './Settings/Settings';
import NoMatch from './NoMatch';

import reducers from '../reducers';
import configureStore from '../Store';

import DevTools from './DevTools';

import GLOBALS from '../GLOBALS';

@Radium
export default class Root extends Component {
  constructor(props) {
    super(props);
    this.history = createHistory();

    const reducer = combineReducers(
      Object.assign(
        {},
        reducers,
        {
          routing,
        }
      )
    );

    const middleware = syncHistory(this.history);

    this.store = configureStore(GLOBALS);

    const finalCreateStore = compose(
      applyMiddleware(middleware),
      DevTools.instrument()
    )(this.store);

    middleware.syncHistoryToStore(this.store);
  }

  render() {
    return (
      <Provider store={this.store}>
        <div>
          <Router history={this.history}>
            <Route path='/' component={App}>
              <IndexRoute component={About}/>
              <Route path='pixels' component={PixelEditor}/>
              <Route path='settings' component={Settings}/>
              <Route path='*' component={NoMatch}/>
            </Route>
          </Router>
          <DevTools store={this.store} />
        </div>
      </Provider>
    );
  }
}
