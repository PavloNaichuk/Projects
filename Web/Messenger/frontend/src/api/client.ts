import { API_BASE_URL } from "./config";

type ApiMethod = "POST" | "PATCH" | "DELETE";

type ApiRequestOptions = {
  accessToken?: string;
  method?: ApiMethod;
  body?: BodyInit;
  json?: unknown;
  errorMessage: string;
  parseError?: (response: Response) => Promise<Error>;
  parseJson?: boolean;
};

function getRequestHeaders(
  accessToken: string | undefined,
  hasJsonBody: boolean
): HeadersInit {
  const headers: Record<string, string> = {};

  if (hasJsonBody) {
    headers["Content-Type"] = "application/json";
  }

  if (accessToken) {
    headers.Authorization = `Bearer ${accessToken}`;
  }

  return headers;
}

export async function apiRequest<T>(
  path: string,
  {
    accessToken,
    method,
    body,
    json,
    errorMessage,
    parseError,
    parseJson = true,
  }: ApiRequestOptions
): Promise<T> {
  const hasJsonBody = json !== undefined;

  const requestInit: RequestInit = {
    headers: getRequestHeaders(accessToken, hasJsonBody),
  };

  if (method) {
    requestInit.method = method;
  }

  if (hasJsonBody) {
    requestInit.body = JSON.stringify(json);
  } else if (body) {
    requestInit.body = body;
  }

  const response = await fetch(`${API_BASE_URL}${path}`, requestInit);

  if (!response.ok) {
    if (parseError) {
      throw await parseError(response);
    }

    throw new Error(errorMessage);
  }

  if (!parseJson) {
    return undefined as T;
  }

  return response.json();
}
