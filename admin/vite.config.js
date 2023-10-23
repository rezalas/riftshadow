import { defineConfig } from "vite";
import laravel from "laravel-vite-plugin";
import vue from "@vitejs/plugin-vue";
import { viteStaticCopy } from "vite-plugin-static-copy";

export default defineConfig({
	resolve: { alias: { vue: "vue/dist/vue.esm-bundler.js" } },
	plugins: [
		laravel([
			"resources/js/app.js",
			"resources/js/registration.js",
			"resources/js/vendor/installer/finished.js",
			"resources/sass/app.scss",
			"resources/sass/vendor/installer/style.scss",
		]),
		vue({
			template: {
				transformAssetUrls: {
					// The Vue plugin will re-write asset URLs, when referenced
					// in Single File Components, to point to the Laravel web
					// server. Setting this to `null` allows the Laravel plugin
					// to instead re-write asset URLs to point to the Vite
					// server instead.
					base: null,

					// The Vue plugin will parse absolute URLs and treat them
					// as absolute paths to files on disk. Setting this to
					// `false` will leave absolute URLs un-touched so they can
					// reference assets in the public directory as expected.
					includeAbsolute: false,
				},
			},
		}),
		viteStaticCopy({
			targets: [
				{
					src: "resources/img/favicon",
					dest: "installer/img",
				},
			],
		}),
	],
});
