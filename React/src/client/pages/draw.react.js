import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg} from '../intl/store';
import Page from '../components/page.react.js';

class DrawPage extends Component {
  render() {
    return (
      <DocumentTitle title={msg('pages.draw.title')}>
        <Page page='draw'></Page>
      </DocumentTitle>
    );
  }
}

export default DrawPage;
