import {input, button, errorMessage} from './colors';

export default {
  form: {
    width: '100%',
  },
  field: {
    textAlign: 'right',
    maxWidth: '50%',
    margin: '0 auto 1em',
  },
  input: {
    fontSize: '1.2em',
    borderBottom: `1px solid ${input.borderBottom}`,
    borderRight: `3px solid ${input.borderRight}`,
    color: input.color,
    padding: '.4em .4em 0 .4em',
    backgroundColor: input.backgroundColor,
    borderRadius: '.3em',
    '-webkit-box-shadow': `0 0 0 50em ${input.backgroundColor} inset`,
    maxWidth: '70%',
  },
  textarea: {
    fontSize: '1.2em',
    borderBottom: '1px solid ${input.borderBottom}',
    borderRight: '3px solid ${input.borderRight}',
    color: input.color,
    padding: '.4em .4em 0 .4em',
    backgroundColor: input.backgroundColor,
    borderRadius: '.3em',
    WebkitBoxShadow: `0 0 0 50em ${input.backgroundColor} inset`,
  },
  buttonContainer: {
    maxWidth: '50%',
    textAlign: 'right',
    margin: '0 auto',
  },
  button: {
    fontSize: '1.2em',
    borderBottom: `1px solid ${button.borderBottom}`,
    borderRight: `3px solid ${button.borderRight}`,
    borderTop: 0,
    borderLeft: 0,
    color: button.color,
    padding: '.4em .4em 0 .4em',
    backgroundColor: button.backgroundColor,
    borderRadius: '.3em',
  },
  checkbox: {

  },
  errorMessage: {
    color: errorMessage.color,
  },
};
