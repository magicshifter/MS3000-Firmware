import Promise from 'bluebird';
import setToString from '../../lib/settostring';
import {ValidationError} from '../../lib/validation';
import {dispatch} from '../../dispatcher';
import {validate} from '../../validation';
import {msg} from '../../intl/store';

export function saveServer(fields) {
  // Promise, because we don't know whether fields are valid.
  const promise = validateServerForm(fields)
    .catch(error => {
      saveFormError(error);
      throw error;
    });

  return dispatch(saveServer, promise);
}

function validateServerForm(fields) {
  // Validate function is just wrapper for node-validator providing promise api,
  // so we can mix client sync and server async validations easily.
  return validate(fields)
    .prop('hostname').required()
    .prop('port').required()
    .promise;
}

export function saveFormError(error) {
  dispatch(saveFormError, error);
}

export function updateFormField({target: {name, value}}) {
  // hostname max length is 31, port maxlength is 5
  let maxLength = 31;
  if (name === 'port') {
    maxLength = 5;
  }

  value = value.slice(0, maxLength);

  dispatch(updateFormField, {name, value});
}

setToString('serverSettings', {
  saveServer,
  saveFormError,
  updateFormField,
});
