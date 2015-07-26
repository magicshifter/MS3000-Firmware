import Promise from 'bluebird';
import setToString from '../../lib/settostring';
import {ValidationError} from '../../lib/validation';
import {dispatch} from '../../dispatcher';
import {validate} from '../../validation';
import {msg} from '../../intl/store';

export function saveAccesspoint(fields) {
  // Promise, because we don't know whether fields are valid.
  const promise = validateAccesspointForm(fields)
    .catch(error => {
      saveFormError(error);
      throw error;
    });

  return dispatch(saveAccesspoint, promise);
}

function validateAccesspointForm(fields) {
  // Validate function is just wrapper for node-validator providing promise api,
  // so we can mix client sync and server async validations easily.
  return validate(fields)
    .prop('ssid').required()
    .prop('password')
    .promise;
}

export function saveFormError(error) {
  dispatch(saveFormError, error);
}

export function updateFormField({target: {name, value}}) {
  // ssid max length is 31
  let maxLength = 31;
  if (name === 'ssid') {
    value = value.slice(0, maxLength);
  }

  dispatch(updateFormField, {name, value});
}

setToString('accesspointSettings', {
  saveAccesspoint,
  saveFormError,
  updateFormField,
});
