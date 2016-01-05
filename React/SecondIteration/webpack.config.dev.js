var path = require('path');
var webpack = require('webpack');

module.exports = {
  devtool: 'cheap-module-eval-source-map',
  entry: [
    'eventsource-polyfill', // necessary for hot reloading with IE
    'webpack-hot-middleware/client',
    './src/index'
  ],
  output: {
    path: path.join(__dirname, 'dist', 'js'),
    filename: 'bundle.js',
    publicPath: '/js/'
  },
  plugins: [
    new webpack.HotModuleReplacementPlugin(),
    new webpack.NoErrorsPlugin()
  ],
  module: {
    loaders: [
      {
        test: /\.(jsx|js)$/,
        loader: ['babel'],
        include: path.join(__dirname, 'src'),
        query: {
          presets: ['react', 'es2015'],
          plugins: ['transform-runtime', 'transform-class-properties', 'transform-decorators-legacy',],
          cacheDirectory: true,
        },
      },
      {
        test: /\.(gif|jpg|png|woff|woff2|eot|ttf|svg)$/,
        loader: 'url-loader?limit=100000',
      },
    ],
  },
};
