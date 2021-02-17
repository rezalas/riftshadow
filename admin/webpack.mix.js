const mix = require("laravel-mix");

/*
 |--------------------------------------------------------------------------
 | Mix Asset Management
 |--------------------------------------------------------------------------
 |
 | Mix provides a clean, fluent API for defining some Webpack build steps
 | for your Laravel application. By default, we are compiling the Sass
 | file for the application as well as bundling up all the JS files.
 |
 */

mix.js("resources/js/app.js", "public/js")
	.js("resources/js/registration.js", "public/js")
	.js("resources/js/vendor/installer/finished.js", "public/installer/js")
	.vue()
	.sass("resources/sass/vendor/installer/style.scss", "public/installer/css")
	.sass("resources/sass/app.scss", "public/css");
