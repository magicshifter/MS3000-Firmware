export default {
  en: {
    header: {
      h1Html: 'MagicShifter',
    },
    menu: {
      home: 'Home',
      leds: 'Leds',
      draw: 'Draw',
      login: 'Login',
      logout: 'Logout',
      settings: 'Settings',
    },
    footer: {
      copyright: '&copy; 2014-2015 MagicShifter.net',
    },
    forms: {
      login: {
        hint: 'Hint: pass1',
        legend: `
          If you do not have a login yet,
          you can try to politely ask the owner to get yourself registered
        `,
        email: {
          label: 'Email:',
          placeholder: 'your@email.com',
        },
        password: {
          label: 'Password:',
          placeholder: 'password',
        },
        wrongPassword: 'Wrong password',
      },
      settings: {
        server: {
          title: 'WebServer Settings',
          legend: 'Change webserver settings',
          placeholder: {
            hostname: 'webserver hostname',
            port: 'webserver port',
          },
          label: {
            hostname: 'Hostname:',
            port: 'Port:',
          },
        },
        accesspoint: {
          title: 'AccessPoint Settings',
          legend: 'Change accesspoint settings',
          placeholder: {
            ssid: 'ssid for the MagicShifter Network',
            password: 'Password',
          },
          label: {
            ssid: 'SSID:',
            password: 'Password:',
          },
        },
      },
    },
    buttons: {
      cancel: 'Cancel',
      edit: 'Edit',
      save: 'Save',
      login: 'Login',
      signup: 'Sign up',
    },
    confirmations: {
      cancelEdit: `You have unsaved changes. Are you sure you want to cancel them?`,
    },
    pages: {
      home: {
        title: 'Home',
        header: 'Home',
        content: `
          This is the MagicShifter Application, it will help set up your
          MagicShifter and you can even use the {ledsLink} page
          to live edit the led colors as well as the
          {paintLink} to create and edit images.
        `,
      },
      leds: {
        title: 'Leds',
        header: 'Leds',
        content: `
          You can edit the leds here.
          If your MagicShifter is online it will react to your inputs
        `,
        menu: {
          title: 'Select:',
          all: 'All',
          none: 'None',
        },
      },
      draw: {
        title: 'Draw',
        header: 'Draw',
        content: `
          Someday soon one will draw beautiful images here.
          And one will rest and look at one's work and will be happy.
        `,
      },
      login: {
        title: 'Login',
        header: 'Login',
        content: `
          After logging in you will be able to control the MagicShifter.
        `,
        bubble: {
          text: 'Welcome to the MagicShifter Control Panel',
        },
      },
      settings: {
        title: 'Settings',
        header: 'Settings',
      },
      notFound: {
        continueMessage: 'Continue here please.',
        header: 'This page is not available',
        message: 'The link may be broken, or the page may have been removed.',
        title: 'Page Not Found',
      },
    },
    validation: {
      email: `Email address is not valid!`,
      password: `Password must contain at least {minLength} characters.`,
      required: `Please fill out {prop, select,
        email {email}
        password {password}
        other {'{prop}'}
      }.`,
      ssid: `ssid can only contain a combination of a-z, A-Z and 0-9`,
    },
    colorPicker: {
      add: {
        title: 'Save selected color',
        text: 'Save color',
      },
      remove: {
        title: 'Remove this color',
        text: 'x',
      },
    },
    debug: {
      continuehappily: 'Continue Happily, it is just a warning',
    },
  },
};
