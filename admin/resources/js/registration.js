import Vue from "vue";
import { axios } from "./bootstrap";
require("./bootstrap");

window.addEventListener("load", function() {
	new Vue({
		el: "#registration",
		data: {
			player_files: [],
			player_passes: {},
			selected_files: []
		},
		methods: {
			resetPlayerFiles() {
				this.player_passes = {};
				this.selected_files = [];
			}
		},
		mounted() {
			axios.get("").then(({ data }) => {
				this.player_files = data.player_files;
			});
		}
	});
});
