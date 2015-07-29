import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';

import styles from './app.styles';

export default class Footer extends Component {

  render() {
    return (
      <footer style={styles.footer}>
        <FormattedHTMLMessage message={msg('footer.copyright')} />
      </footer>
    );
  }
}
