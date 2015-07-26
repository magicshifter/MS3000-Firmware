import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';

class Footer extends Component {

  render() {
    const styles = {
      footer: {
        fontSize: '.75em',
        fontStyle: 'italic',
        position: 'absolute',
        bottom: 0,
      },
    };
    return (
      <footer style={styles.footer}>
        <FormattedHTMLMessage message={msg('footer.copyright')} />
      </footer>
    );
  }
}

export default Footer;
