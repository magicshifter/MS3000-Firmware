import Promise from 'bluebird';
import setToString from '../lib/settostring';
import {ValidationError} from '../lib/validation';
import {dispatch} from '../dispatcher';
import {validate} from '../validation';
import {msg} from '../intl/store';
import fetch from 'isomorphic-fetch';

export function login(fields) {
  // Promise, because we don't know whether fields are valid.
  const promise = validateForm(fields)
    .then(() => {
      return validateCredentials(fields);
    })

    .catch(error => {
      loginError(error);
      throw error;
    });

  return dispatch(login, promise);
}

function validateForm(fields) {
  // Validate function is just wrapper for node-validator providing promise api,
  // so we can mix client sync and server async validations easily.

  return validate(fields)
    .prop('email').required().email()
    .prop('password').required().simplePassword()
    .promise;
}

function validateCredentials(fields) {
  return new Promise((resolve, reject) => {
    fetch('/api/v1/auth/login', {
      method: 'post',
      headers: {
        Accept: 'application/json',
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(fields),
    })
    .then((res) => res.status)
    .then((status) => {
      if (status === 200) {
        resolve(fields);
      } else {
        reject(
          new ValidationError(msg('forms.login.wrongPassword'), 'password')
        );
      }
    });
  });
}

export function loginError(error) {
  dispatch(loginError, error);
}

export function logout() {
  // Always reload app on logout for security reasons.
  location.href = '/';
}

export function updateFormField({target: {name, value}}) {
  // Both email and password max length is 100.
  value = value.slice(0, 100);
  dispatch(updateFormField, {name, value});
}

setToString('auth', {
  login,
  loginError,
  logout,
  updateFormField,
});
