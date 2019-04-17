using System.Collections.Generic;
using System.Web.Http;

namespace SocialNetworkWebAPI.Controllers
{
    [Authorize]
    public class MessageController : ApiController
    {
        // GET api/message
        public IEnumerable<string> Get()
        {
            return new string[] { "messages" };
        }

        // GET api/message/5
        public string Get(int id)
        {
            return "message";
        }

        // POST api/message
        public void Post([FromBody]string message)
        {
        }

        // PUT api/message/5
        public void Put(int id, [FromBody]string message)
        {
        }

        // DELETE api/message/5
        public void Delete(int id)
        {
        }

    }
}
