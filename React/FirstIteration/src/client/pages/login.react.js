import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import LoginForm from '../auth/login.react';
import React from 'react';
import {msg} from '../intl/store';
import Title from '../components/page/title.react';
import Content from '../components/page/content.react';
import Bubble from '../components/bubble.react';

export default class Login extends Component {

  render() {
    return (
      <article className='login'>
        <Title page='login' />
        <Bubble text={msg('pages.login.bubble.text')} />
        <Content page='login' />
        <LoginForm {...this.props} />
      </article>
    );
  }
}
