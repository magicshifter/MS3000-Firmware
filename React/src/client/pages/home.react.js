import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg, FormattedMessage} from '../intl/store';

class Home extends Component {

  static propTypes = {
    isLoggedIn: React.PropTypes.bool.isRequired
  };

  render() {
    return (
      <DocumentTitle title={msg('pages.home.title')}>
        <div className="home-page">
          <h2>{msg('pages.home.header')}</h2>
          <div className='content'>{msg('pages.home.content')}</div>
        </div>
      </DocumentTitle>
    );
  }
}

export default Home;
