const { GCLoad } = require('..');
const gcLoad = new GCLoad();
gcLoad.start();
setInterval(() => {
    for (let i = 0; i < 1000; i++) {
        new Array(100);
    }
    gc();
    console.log(gcLoad.total());
}, 3000);