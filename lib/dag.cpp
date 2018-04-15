#include "dag.h"

SetBaseUrl::SetBaseUrl(bool https, std::string host, std::string path) {
    this->https = https;
    this->host = host;
    this->path = path;
}

bool SetBaseUrl::forward(State *state) const {
    state->https = this->https;
    state->host = this->host;

    // TODO: memcpy?
    for (size_t i = 0; i < this->path.size(); i++) {
        state->path.push_back(this->path[i]);
    }

    // TODO: Open connection?

    return true;
}

void SetBaseUrl::backward(State *state) const {
    state->https = false;
    state->host = "";

    // TODO: O(1)?
    for (size_t i = 0; i < this->path.size(); i++) {
        state->path.pop_back();
    }

    // TODO: Close connection?
}

AppendPath::AppendPath(std::string append) {
    this->append = append;
}

bool AppendPath::forward(State *state) const {
    // TODO: memcpy?
    for (size_t i = 0; i < this->append.size(); i++) {
        state->path.push_back(this->append[i]);
    }
    return true;
}

void AppendPath::backward(State *state) const {
    // TODO: O(1) ?
    for (size_t i = 0; i < this->append.size(); i++) {
        state->path.pop_back();
    }
}

PerformRequest::PerformRequest(IO *io, int w) {
    this->io = io;
    this->w = w;
}

bool PerformRequest::forward(State *state) const {
    usleep(w * 1000);

    std::stringstream ss;
    for (size_t i = 0; i < state->path.size(); i++) {
        ss << state->path[i];
    }

    auto lock = this->io->get_write_lock();
    this->io->write_string("response");
    this->io->write_string(std::string(state->https ? "https://" : "http://") + state->host + ss.str());
    return true;
}

void PerformRequest::backward(State *state) const {
}

