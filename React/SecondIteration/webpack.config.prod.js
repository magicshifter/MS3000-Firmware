var path = require('path');
var webpack = require('webpack');
var CopyWebpackPlugin = require('copy-webpack-plugin');

module.exports = {
  devtool: 'source-map',
  entry: [
    './index'
  ],
  context: path.join(__dirname, 'src'),
  output: {
    path: path.join(__dirname, 'dist'),
    filename: 'js/bundle.js',
    publicPath: '/js/'
  },
  plugins: [
    new webpack.optimize.OccurenceOrderPlugin(),
    new webpack.DefinePlugin({
      'process.env': {
        'NODE_ENV': JSON.stringify('production')
      }
    }),
    new webpack.optimize.UglifyJsPlugin({
      compressor: {
        warnings: false
      }
    })
  ],
  module: {
     loaders: [
      {
        test: /\.(jsx|js)$/,
        loader: ['babel'],
        include: path.join(__dirname, 'src'),
        query: {
          presets: ['react', 'es2015'],
          plugins: ['transform-class-properties', 'transform-decorators-legacy',],
          cacheDirectory: true,
        },
      },
      {
        test: /\.(gif|jpg|png|woff|woff2|eot|ttf|svg)$/,
        loader: 'url-loader?limit=100000',
      },
    ],
  },
  plugins: [
    new CopyWebpackPlugin([
      {
        from: 'assets',
      },
    ]),
  ],
};
