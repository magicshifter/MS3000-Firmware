import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Page from '../components/page.react';

export default class NotFound extends Component {

  render() {
    const append = <Link to='home'>
                      {msg('pages.notFound.continueMessage')}
                    </Link>;

    return (
      <article className='notfound'>
        <Page page='notfound' append={append}></Page>
      </article>
    );
  }
}
