import { createApp } from "vue";
import { axios } from "./bootstrap";
import "./bootstrap";

window.addEventListener("load", function () {
	createApp({
		data() {
			return {
				player_files: [],
				player_passes: {},
				selected_files: [],
			};
		},
		methods: {
			resetPlayerFiles() {
				this.player_passes = {};
				this.selected_files = [];
			},
		},
		mounted() {
			axios.get("").then(({ data }) => {
				this.player_files = data.player_files;
			});
		},
	}).mount("#registration");
});
