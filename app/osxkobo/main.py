#!/usr/bin/env python
#
# Copyright 2007 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
from google.appengine.ext import webapp
from google.appengine.ext.webapp import util
import os

class MainHandler(webapp.RequestHandler):
    def get(self):
        filename = 'index.html'
        for i in self.request.accept_language.best_matches('en'):
            if i == 'ko':
                filename = 'index.ko.html'
                break
            if i == 'ja':
                filename = 'index.ja.html'
                break
#        filename = os.path.join(os.path.split(__file__)[0], 'site', filename) 
        f = open(filename, 'r')
        self.response.out.write(f.read())
        f.close


def main():
    application = webapp.WSGIApplication([('/', MainHandler)],
                                         debug=True)
    util.run_wsgi_app(application)


if __name__ == '__main__':
    main()
