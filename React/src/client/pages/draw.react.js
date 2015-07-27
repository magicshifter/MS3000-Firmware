import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg} from '../intl/store';
import PageTitle from '../components/pagetitle.react.js';

class DrawPage extends Component {
  render() {
    return (
      <DocumentTitle title={msg('pages.draw.title')}>
        <div className="draw-page">
          <PageTitle page='draw'></PageTitle>
        </div>
      </DocumentTitle>
    );
  }
}

export default DrawPage;
