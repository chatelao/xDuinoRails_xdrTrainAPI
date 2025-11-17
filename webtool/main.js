document.addEventListener('DOMContentLoaded', () => {
    const targetUrlInput = document.getElementById('target-url');
    const messageTypeSelect = document.getElementById('message-type');
    const messageBodyTextarea = document.getElementById('message-body');
    const sendButton = document.getElementById('send-button');
    const responsePre = document.getElementById('response');

    let openapiSpec = null;

    // Function to fetch and parse the OpenAPI spec
    const loadOpenApiSpec = async () => {
        try {
            const response = await fetch('../swagger/openapi.yaml');
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            const yamlText = await response.text();
            openapiSpec = jsyaml.load(yamlText);
            console.log('OpenAPI spec loaded successfully:', openapiSpec); // Debug log
            populateMessageTypeDropdown();
            updateMessageBody(); // Initial population
        } catch (error) {
            console.error('Error loading OpenAPI spec:', error);
            // Make the error very visible for debugging
            document.body.innerHTML = `<pre>FATAL ERROR: Could not load OpenAPI spec.\n${error.stack}</pre>`;
        }
    };

    // Function to populate the message type dropdown
    const populateMessageTypeDropdown = () => {
        if (!openapiSpec || !openapiSpec.paths) {
            return;
        }

        for (const path in openapiSpec.paths) {
            const option = document.createElement('option');
            const postOperation = openapiSpec.paths[path].post;
            if (postOperation) {
                const summary = postOperation.summary || path.substring(1); // Use summary or path name
                option.value = path;
                option.textContent = summary;
                messageTypeSelect.appendChild(option);
            }
        }
    };

    // Function to update the message body with an example
    const updateMessageBody = () => {
        if (!openapiSpec) return;

        const selectedPath = messageTypeSelect.value;
        const pathSpec = openapiSpec.paths[selectedPath];

        if (pathSpec && pathSpec.post && pathSpec.post.requestBody) {
            const requestBody = pathSpec.post.requestBody;
            const content = requestBody.content['application/json'];
            if (content && content.examples) {
                // Get the first example
                const firstExampleKey = Object.keys(content.examples)[0];
                const exampleValue = content.examples[firstExampleKey].value;
                console.log('Updating message body with example:', exampleValue); // Debug log
                messageBodyTextarea.value = JSON.stringify(exampleValue, null, 2);
            } else if (content && content.schema) {
                // If no example, create a placeholder from the schema (simplified)
                 const exampleValue = createObjectFromSchema(content.schema);
                 console.log('Updating message body with schema:', exampleValue); // Debug log
                 messageBodyTextarea.value = JSON.stringify(exampleValue, null, 2);
            }
        } else {
            console.log('No example or schema found for selected path:', selectedPath); // Debug log
            messageBodyTextarea.value = '';
        }
    };

    // Helper function to create a placeholder object from a schema
    const createObjectFromSchema = (schemaRef) => {
        const schemaName = schemaRef['$ref'].split('/').pop();
        const schema = openapiSpec.components.schemas[schemaName];
        const obj = {};
        if (schema && schema.properties) {
            for (const prop in schema.properties) {
                const propSchemaRef = schema.properties[prop]['$ref'];
                if(propSchemaRef) {
                    obj[prop] = createObjectFromSchema(schema.properties[prop]);
                } else {
                    obj[prop] = schema.properties[prop].example || '';
                }
            }
        }
        return obj;
    };


    // Function to handle sending the message
    const sendMessage = async () => {
        const targetUrl = targetUrlInput.value;
        const messageType = messageTypeSelect.value;
        const messageBody = messageBodyTextarea.value;
        const fullUrl = `${targetUrl}${messageType}`;

        responsePre.textContent = 'Sending...';

        try {
            const response = await axios.post(fullUrl, JSON.parse(messageBody), {
                headers: {
                    'Content-Type': 'application/json'
                }
            });
            responsePre.textContent = JSON.stringify(response.data, null, 2);
        } catch (error) {
             if (error.response) {
                // The request was made and the server responded with a status code
                // that falls out of the range of 2xx
                responsePre.textContent = `Error: ${error.message}\n\nStatus: ${error.response.status}\n\nResponse:\n${JSON.stringify(error.response.data, null, 2)}`;
            } else if (error.request) {
                // The request was made but no response was received
                 responsePre.textContent = `Error: No response received from the server. Check the URL and network connection.\n\n${error.message}`;
            } else {
                // Something happened in setting up the request that triggered an Error
                responsePre.textContent = `Error: ${error.message}`;
            }
        }
    };

    // Event Listeners
    messageTypeSelect.addEventListener('change', updateMessageBody);
    sendButton.addEventListener('click', sendMessage);

    // Initial load
    loadOpenApiSpec();
});
