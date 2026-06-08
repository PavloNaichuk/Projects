import { API_BASE_URL } from "./config";

type ApiMethod = "POST" | "PATCH" | "DELETE";

type ApiRequestOptions = {
  accessToken: string;
  method?: ApiMethod;
  body?: BodyInit;
  json?: unknown;
  errorMessage: string;
};

function getAuthHeaders(accessToken: string, hasJsonBody: boolean): HeadersInit {
  const headers: Record<string, string> = {
    Authorization: `Bearer ${accessToken}`,
  };

  if (hasJsonBody) {
    headers["Content-Type"] = "application/json";
  }

  return headers;
}

export async function apiRequest<T>(
  path: string,
  { accessToken, method, body, json, errorMessage }: ApiRequestOptions
): Promise<T> {
  const hasJsonBody = json !== undefined;

  const requestInit: RequestInit = {
    headers: getAuthHeaders(accessToken, hasJsonBody),
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
    throw new Error(errorMessage);
  }

  return response.json();
}
