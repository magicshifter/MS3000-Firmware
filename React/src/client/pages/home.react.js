import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg, FormattedMessage} from '../intl/store';
import Page from '../components/page.react.js';

class Home extends Component {

  render() {
    return (
      <article className='home'>
        <Page page='home'></Page>
      </article>
    );
  }
}

export default Home;
