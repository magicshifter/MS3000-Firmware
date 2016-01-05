import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg} from '../intl/store';
import Page from '../components/page.react.js';

export default class DrawPage extends Component {
  render() {
    return (
      <article className='home'>
        <Page page='draw' />
      </article>
    );
  }
}
