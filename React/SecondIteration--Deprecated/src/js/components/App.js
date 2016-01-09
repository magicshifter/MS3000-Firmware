import React from 'react';
import { Link } from 'react-router';
import { connect } from 'react-redux';
import { pushPath } from 'redux-simple-router';

import Header from './Header';

function App({ pushPath, children }) {
  return (
    <div>
      <Header />
      <div>
        {children}
      </div>
    </div>
  );
};

const conn = connect(
  null,
  { pushPath }
)(App);

export default conn;
