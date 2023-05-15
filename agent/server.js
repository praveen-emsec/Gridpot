const express = require('express');

//Axios
const axios = require('axios');

//Constants
const PORT = 9090;
const HOST = '0.0.0.0';

//App
const app = express();
app.use(express.json());
app.disable('x-powered-by');

app.get('/', (req, res) => {
    res.send('Agent Up');
});

app.get('/status', async (req, res) => {
    const result = await axios.get('http://elasticsearch:9200/')
    .then(response => {
        return response;
    }).catch(err => {
	console.log(err);
    });
    console.log(result.data);
    res.status(200).json(result.data);
});

app.post('/search1', async (req, res) => {
    const body = req.body;
    //console.log(req);
    console.log(body);
    const result = await axios.post('http://elasticsearch:9200/filebeat-7.7.0/_search', body)
    .then(response => {
        return response;
    }).catch(err => {
        console.log(err);
    });
    console.log(result.data);
    res.status(200).json(result.data);
});

app.listen(PORT, HOST, () => {
    console.log(`Runing on ${HOST}:${PORT}`);
    //Add Agent Up Webhook Call to Command Center
});
