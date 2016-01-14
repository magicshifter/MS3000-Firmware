import React, {PropTypes, Component} from 'react';
import { Provider } from 'react-redux';
import { Router } from 'react-router';

import RadiumContainer from 'containers/RadiumContainer';

export default class Root extends Component {
  static propTypes = {
    history: PropTypes.object.isRequired,
    routes: PropTypes.element.isRequired,
    store: PropTypes.object.isRequired,
  };

  get content() {
    return (
      <RadiumContainer>
        <Router history={this.props.history}>
          {this.props.routes}
        </Router>
      </RadiumContainer>
    );
  }

  get devTools() {
    if (__DEBUG__) {
      if (__DEBUG_NEW_WINDOW__) {
        if (!window.devToolsExtension) {
          require('../redux/utils/createDevToolsWindow').default(this.props.store);
        } else {
          window.devToolsExtension.open();
        }
      } else if (!window.devToolsExtension) {
        const DevTools = require('containers/DevTools').default;
        return (
          <DevTools />
        );
      }
    }
  }

  render() {
    const {store} = this.props;
    const state = store.getState();

    const fontSize = state.layout.get('fontSize');

    return (
      <Provider store={store}>
        <div
          style={{
            fontSize,
          }}
        >
          {this.content}
          {this.devTools}
        </div>
      </Provider>
    );
  }
}
