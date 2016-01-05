import {input, button, legend, errorMessage} from './colors';

export default {
  form: {
    width: '100%',
  },
  fieldset: {
    maxWidth: '60%',
    margin: '0 auto',
  },
  field: {
    textAlign: 'right',
    margin: '1em auto',
  },
  input: {
    fontSize: '1.2em',
    borderBottom: `1px solid ${input.borderBottom}`,
    borderRight: `3px solid ${input.borderRight}`,
    color: input.color,
    padding: '.4em .4em 0 .4em',
    backgroundColor: input.backgroundColor,
    borderRadius: '.3em',
    WebkitBoxShadow: `0 0 0 50em ${input.backgroundColor} inset`,
    maxWidth: '70%',
    margin: '0 0 0 .5em',
  },
  legend: {
    color: legend.color,
    fontStyle: 'italic',
    fontSize: '.8em',
    margin: '0 0 .5em',
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
    textAlign: 'right',
    margin: '0 auto 1em',
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
  errorMessage: {
    color: errorMessage.color,
  },
};
