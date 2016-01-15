import React from 'react';

export default () => {
  const updateUrl = 'http://magicshifter.net/ms3000/update';

  return (
    <div className='container text-center'>
      <h1>Welcome to the MagicShifter 3000 Web Interface!</h1>
      <p>
        Although we did our best to make this first beta firmware as good as possible,
        we also know that it is far from being finished.
      </p>
      <p>
        The new MS3000 hardware allows so many new use cases
        for connected apps that it will take us some time to implement our
        vision of the perfect MS3000 user interface one feature at a time.
      </p>
      <p>
        Expect the first firmware update in February 2016.
        When it's done you can always find the latest firmware
        (and instructions on how to update) at: <a href={updateUrl}>{updateUrl}</a>
      </p>

      <p>Greetings,</p>
      <p>The MagicShifter Team!</p>
    </div>
  );
};
