import React, {PropTypes, Component} from 'react';
import { Provider } from 'react-redux';
import { Router } from 'react-router';

export default class Root extends Component {
  static propTypes = {
    history: PropTypes.object.isRequired,
    routes: PropTypes.element.isRequired,
    store: PropTypes.object.isRequired,
  };

  get content() {
    return (
      <Router history={this.props.history}>
        {this.props.routes}
        {this.devTools}
      </Router>
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

    return (
      <Provider store={store}>
        {this.content}
      </Provider>
    );
  }
}
