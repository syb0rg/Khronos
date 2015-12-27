#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include "internet.h"

#define BUF_SIZE 0x1000
#define RESPONSE_SIZE 0x100
#define KEY "AIzaSyA4IqJgF0Z2v4bEE9M98o8mMBw6Z9cRND8"

void freeResponse(ServerResponse *resp)
{
	if (resp) 
	{
		free(resp->data);
		free(resp);
	}
	resp = NULL;
}

ServerResponse *sendAudioData(void *data, int length, const char *language, uint32_t sample_rate)
{
	CURL *conn_hndl;
	struct curl_httppost *form, *lastptr;
	struct curl_slist *headers;
	ServerResponse *resp;
	char url[0x100];
	char header[0x100];

	if (!data) return NULL;

	/*
	 * Initialize the variables
	 * Put the language code to the URL query string
	 * If no language given, default to U. S. English
	 */
	snprintf(url, sizeof(url), "https://www.google.com/speech-api/v2/recognize?client=chromium&output=json&lang=%s&key=%s", language ? language : "en-US", KEY);
	resp = malloc(sizeof(*resp));
	if (!resp) return NULL;

	resp->data = (char*) malloc(RESPONSE_SIZE);
	if (!resp->data)
	{
		freeResponse(resp);
		return NULL;
	}

	resp->length = 0;
	conn_hndl = curl_easy_init();
	if (!conn_hndl)
	{
		freeResponse(resp);
		return NULL;
	}

	form = NULL;
	lastptr = NULL;
	headers = NULL;
	snprintf(header, sizeof(header), "Content-Type: audio/x-flac; rate=%lu", (unsigned long)sample_rate);
	headers = curl_slist_append(headers, header);

	curl_formadd(&form, &lastptr, CURLFORM_COPYNAME, "myfile", CURLFORM_CONTENTSLENGTH, length, CURLFORM_PTRCONTENTS, data, CURLFORM_END);

	// Setup the cURL handle
	curl_easy_setopt(conn_hndl, CURLOPT_URL, url);
	curl_easy_setopt(conn_hndl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(conn_hndl, CURLOPT_HTTPPOST, form);
	curl_easy_setopt(conn_hndl, CURLOPT_WRITEFUNCTION, httpCallback);

	// SSL certificates are not available on iOS, so we have to trust Google
	curl_easy_setopt(conn_hndl, CURLOPT_SSL_VERIFYPEER, 0);

	// Initiate the HTTP(S) transfer
	curl_easy_perform(conn_hndl);

	// Clean up
	curl_formfree(form);
	curl_slist_free_all(headers);
	curl_easy_cleanup(conn_hndl);

	// NULL-terminate the JSON response string
	resp->data[resp->length] = '\0';
    
    return resp;
}
/**
 * @fn int testConnection(void)
 * @brief Pings "https://www.google.com/" to test if there is a DNS resolution by connecting to the URL and making an HTTP request.  Timeout is set to three seconds.
 * @return Success value if connected to the internet.
 * @todo Rewrite to use an ICMP request
 */
int testConnection(void)
{
	CURLcode res = CURLE_OK;
	CURL *curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	else res = CURLE_FAILED_INIT;
	curl_global_cleanup();
	return res;
}

/* Map the contents of file into memory: if successful, update
 * *contents to point to the file contents, update *size to be the
 * size of the file, and return 0. Otherwise, return -1.
 */
int getFileContents(const char *file, void **contents, size_t *size)
{
    int fd = open(file, O_RDONLY);
    if (fd >= 0)
    {
        struct stat st = {0};
        if (fstat(fd, &st) == 0)
        {
            void *result = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (result != MAP_FAILED)
            {
                *contents = result;
                *size = st.st_size;
                munmap(NULL, st.st_size);
                close(fd);
                return 0;
            }
        }
        close(fd);
    }
    return -1;
}

size_t httpCallback(char *ptr, size_t count, size_t blocksize, void *userdata)
{
	ServerResponse *response = userdata;
	size_t size = count * blocksize;

	if (response->length + size < RESPONSE_SIZE) {
		// do not write past buffer
		memcpy(response->data + response->length, ptr, size);
		response->length += size;
	}

	return size;
}