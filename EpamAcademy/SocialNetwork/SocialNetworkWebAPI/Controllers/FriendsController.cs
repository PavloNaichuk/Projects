using System.Collections.Generic;
using System.Web.Http;

namespace SocialNetworkWebAPI.Controllers
{
    [Authorize]
    public class FriendsController : ApiController
    {

        // GET api/friends
        public IEnumerable<string> Get()
        {
            return new string[] { "friend"};
        }

        // GET api/friends/5
        public string Get(int friendId)
        {
            return "friend";
        }

        // POST api/friends
        public void Post([FromBody]string friend)
        {
        }

        // PUT api/friends/5
        public void Put(int friendId, [FromBody]string friend)
        {
        }

        // DELETE api/friends/5
        public void Delete(int friendId)
        {
        }

    }
}
