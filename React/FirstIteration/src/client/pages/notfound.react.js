import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg} from '../intl/store';
import Page from '../components/page.react';

export default class NotFound extends Component {

  render() {

    return (
      <article className='notFound'>
        <Page page='notFound' />
      </article>
    );
  }
}
