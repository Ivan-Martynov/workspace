import { defineConfig } from "vite";
import topLevelAwait from "vite-plugin-top-level-await";

export default defineConfig({
    base: "./",
    plugins: [topLevelAwait()],
    optimizeDeps: {
        esbuildOptions: {
            target: "esnext",
        },
    },
    build: {
        target: "esnext",
        rollupOptions: {
            external: /\.skel$/,
        },
        chunkSizeWarningLimit: 1600,
    },
});
